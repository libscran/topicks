<?xml version='1.0' encoding='UTF-8' standalone='yes' ?>
<tagfile doxygen_version="1.12.0">
  <compound kind="file">
    <name>pick_top_genes.hpp</name>
    <path>topicks/</path>
    <filename>pick__top__genes_8hpp.html</filename>
    <class kind="struct">topicks::PickTopGenesOptions</class>
    <namespace>topicks</namespace>
  </compound>
  <compound kind="file">
    <name>topicks.hpp</name>
    <path>topicks/</path>
    <filename>topicks_8hpp.html</filename>
    <includes id="pick__top__genes_8hpp" name="pick_top_genes.hpp" local="yes" import="no" module="no" objc="no">pick_top_genes.hpp</includes>
    <includes id="TopQueue_8hpp" name="TopQueue.hpp" local="yes" import="no" module="no" objc="no">TopQueue.hpp</includes>
    <namespace>topicks</namespace>
  </compound>
  <compound kind="file">
    <name>TopQueue.hpp</name>
    <path>topicks/</path>
    <filename>TopQueue_8hpp.html</filename>
    <class kind="struct">topicks::TopQueueOptions</class>
    <class kind="class">topicks::TopQueue</class>
    <namespace>topicks</namespace>
  </compound>
  <compound kind="struct">
    <name>topicks::PickTopGenesOptions</name>
    <filename>structtopicks_1_1PickTopGenesOptions.html</filename>
    <templarg>typename Stat_</templarg>
    <member kind="variable">
      <type>std::optional&lt; Stat_ &gt;</type>
      <name>bound</name>
      <anchorfile>structtopicks_1_1PickTopGenesOptions.html</anchorfile>
      <anchor>afce1d913773d4cca49ccc07a7f952f88</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>bool</type>
      <name>open_bound</name>
      <anchorfile>structtopicks_1_1PickTopGenesOptions.html</anchorfile>
      <anchor>af66b687b1bc89dee122ab85a27afdf3f</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>bool</type>
      <name>keep_ties</name>
      <anchorfile>structtopicks_1_1PickTopGenesOptions.html</anchorfile>
      <anchor>a2db78cb727b9c6e9c687127ee5bbbb69</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="class">
    <name>topicks::TopQueue</name>
    <filename>classtopicks_1_1TopQueue.html</filename>
    <templarg>typename Stat_</templarg>
    <templarg>typename Index_</templarg>
    <member kind="function">
      <type></type>
      <name>TopQueue</name>
      <anchorfile>classtopicks_1_1TopQueue.html</anchorfile>
      <anchor>a3eb293431b5b55725148d1f24443a90f</anchor>
      <arglist>(const Index_ top, const bool larger, const TopQueueOptions&lt; Stat_ &gt; &amp;options)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>push</name>
      <anchorfile>classtopicks_1_1TopQueue.html</anchorfile>
      <anchor>a26e76a49e4e36fb1fe734bce3222e618</anchor>
      <arglist>(std::pair&lt; Stat_, Index_ &gt; gene)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>emplace</name>
      <anchorfile>classtopicks_1_1TopQueue.html</anchorfile>
      <anchor>a396044984a5dde2833fb7a1ec0048035</anchor>
      <arglist>(Stat_ statistic, Index_ index)</arglist>
    </member>
    <member kind="function">
      <type>Index_</type>
      <name>size</name>
      <anchorfile>classtopicks_1_1TopQueue.html</anchorfile>
      <anchor>afc4a3821ae393344b71fbedddfb481ee</anchor>
      <arglist>() const</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>empty</name>
      <anchorfile>classtopicks_1_1TopQueue.html</anchorfile>
      <anchor>ae94d3c9521913c63e610c0ee07b1fcfe</anchor>
      <arglist>() const</arglist>
    </member>
    <member kind="function">
      <type>const std::pair&lt; Stat_, Index_ &gt; &amp;</type>
      <name>top</name>
      <anchorfile>classtopicks_1_1TopQueue.html</anchorfile>
      <anchor>a3eb0f3afc1a9542736255b808015fb93</anchor>
      <arglist>() const</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>pop</name>
      <anchorfile>classtopicks_1_1TopQueue.html</anchorfile>
      <anchor>a36c1af55c44cbc3f5f2cbdc001435b2c</anchor>
      <arglist>()</arglist>
    </member>
  </compound>
  <compound kind="struct">
    <name>topicks::TopQueueOptions</name>
    <filename>structtopicks_1_1TopQueueOptions.html</filename>
    <templarg>typename Stat_</templarg>
    <member kind="variable">
      <type>std::optional&lt; Stat_ &gt;</type>
      <name>bound</name>
      <anchorfile>structtopicks_1_1TopQueueOptions.html</anchorfile>
      <anchor>a6a382963aafef610e30c921c0705c4e2</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>bool</type>
      <name>open_bound</name>
      <anchorfile>structtopicks_1_1TopQueueOptions.html</anchorfile>
      <anchor>a80d91d1ed3ae89aa9b8e92fb050b350f</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>bool</type>
      <name>keep_ties</name>
      <anchorfile>structtopicks_1_1TopQueueOptions.html</anchorfile>
      <anchor>aa40aaef482e1df7862067b5a71b58a9f</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="namespace">
    <name>topicks</name>
    <filename>namespacetopicks.html</filename>
    <class kind="struct">topicks::PickTopGenesOptions</class>
    <class kind="class">topicks::TopQueue</class>
    <class kind="struct">topicks::TopQueueOptions</class>
    <member kind="function">
      <type>void</type>
      <name>pick_top_genes</name>
      <anchorfile>namespacetopicks.html</anchorfile>
      <anchor>afb96918e66418b1d67caca308c3acc98</anchor>
      <arglist>(const std::size_t n, const Stat_ *const statistic, const std::size_t top, const bool larger, Bool_ *const output, const PickTopGenesOptions&lt; Stat_ &gt; &amp;options)</arglist>
    </member>
    <member kind="function">
      <type>std::vector&lt; Bool_ &gt;</type>
      <name>pick_top_genes</name>
      <anchorfile>namespacetopicks.html</anchorfile>
      <anchor>afdc0dc3243590fe8190e5012c4d01ed9</anchor>
      <arglist>(const std::size_t n, const Stat_ *const statistic, const std::size_t top, const bool larger, const PickTopGenesOptions&lt; Stat_ &gt; &amp;options)</arglist>
    </member>
    <member kind="function">
      <type>std::vector&lt; Index_ &gt;</type>
      <name>pick_top_genes_index</name>
      <anchorfile>namespacetopicks.html</anchorfile>
      <anchor>ae75199c927619278d5871bb73096a877</anchor>
      <arglist>(const Index_ n, const Stat_ *const statistic, const Index_ top, const bool larger, const PickTopGenesOptions&lt; Stat_ &gt; &amp;options)</arglist>
    </member>
  </compound>
  <compound kind="page">
    <name>index</name>
    <title>Pick top genes</title>
    <filename>index.html</filename>
    <docanchor file="index.html" title="Pick top genes">md__2github_2workspace_2README</docanchor>
  </compound>
</tagfile>
